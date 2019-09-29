//
// Created L1MeN9Yu on 2018/5/16.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

typealias GTRSucceedClosure = (_ httpHeader: HttpHeader?, _ responseData: Foundation.Data) -> Void
typealias GTRFailureClosure = (_ httpResponseCode: Swift.Int, _ errorCode: Swift.Int32, _ errorMessage: Swift.String) -> Void
typealias GTRProgressClosure = (_ now: UInt64, _ total: UInt64) -> Void
typealias GTRHttpHeaderClosure = () -> [Swift.String: Swift.String]

final class Engine {
    private(set) var responseQueue = DispatchQueue.main
    private(set) var notificationQueue = DispatchQueue.main
    private(set) var succeedContainer: [Swift.UInt32: GTRSucceedClosure?] = [Swift.UInt32: GTRSucceedClosure?]()
    private(set) var failureContainer: [Swift.UInt32: GTRFailureClosure?] = [Swift.UInt32: GTRFailureClosure?]()
    private(set) var downloadProgressContainer: [Swift.UInt32: GTRProgressClosure?] = [Swift.UInt32: GTRProgressClosure?]()
    private(set) var uploadProgressContainer: [Swift.UInt32: GTRProgressClosure?] = [Swift.UInt32: GTRProgressClosure?]()

    private var httpHeaderClosure: GTRHttpHeaderClosure?

    fileprivate let rwLock = ReadWriteLock()
}

// MARK: - Internal
extension Engine {
    func fire(engineNumber: String? = nil, cylinderCount: UInt32) {
        c_gtr_init(engineNumber?.cString(using: .utf8), cylinderCount)
    }

    // MARK: Request
    @discardableResult
    func request(httpMethod: Method,
                 url: String,
                 headers: [String: Encodable]? = nil,
                 contentType: ContentType = .json,
                 timeOut: UInt32,
                 speedLimit: Int,
                 param: [String: Any]? = nil,
                 downloadPath: String? = nil,
                 progress: GTRProgressClosure? = nil,
                 succeed: GTRSucceedClosure?,
                 failure: GTRFailureClosure?) -> UInt32 {

        switch httpMethod {
        case .get:
            return self.getRequest(url: url, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, succeed: succeed, failure: failure)
        case .post:
            return self.postRequest(url: url, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, param: param, succeed: succeed, failure: failure)
        case .put:
            return self.putRequest(url: url, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, param: param, succeed: succeed, failure: failure)
        case .download:
            guard let downloadPath = downloadPath else { fatalError("must use download path") }
            return self.downloadRequest(url: url, filePath: downloadPath, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, progress: progress, succeed: succeed, failure: failure)
        case .upload:
            //TODO
            fatalError("not implement yet")
        }
    }

// MARK: Config
    func config(httpHeaderClosure: GTRHttpHeaderClosure?) {
        self.httpHeaderClosure = httpHeaderClosure
    }

    func config(responseQueue: DispatchQueue) {
        self.responseQueue = responseQueue
    }

}

// MARK: - FilePrivate
extension Engine {
    fileprivate func cleanResponseHandler(taskID: UInt32) {
        self.succeedContainer.removeValue(forKey: taskID)
        self.failureContainer.removeValue(forKey: taskID)
    }

    fileprivate func cleanDownloadProgress(taskID: UInt32) {
        self.downloadProgressContainer.removeValue(forKey: taskID)
    }

    fileprivate func cleanUploadProgress(taskID: UInt32) {
        self.uploadProgressContainer.removeValue(forKey: taskID)
    }
}

// MARK: - Private
extension Engine {
    private func getRequest(url: String,
                            headers: [String: Encodable]? = nil,
                            contentType: ContentType = .json,
                            timeOut: UInt32,
                            speedLimit: Int,
                            succeed: GTRSucceedClosure?,
                            failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)

        c_gtr_get(&taskID, url.cString(using: .utf8), cHeaders, timeOut, speedLimit)

        self.rwLock.withWriterLock { () -> Void in
            self.succeedContainer[taskID] = succeed
            self.failureContainer[taskID] = failure
        }

        return taskID
    }

    private func postRequest(url: String,
                             headers: [String: Encodable]? = nil,
                             contentType: ContentType = .json,
                             timeOut: UInt32,
                             speedLimit: Int,
                             param: [String: Any]? = nil,
                             succeed: GTRSucceedClosure?,
                             failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)

        var c_param: UnsafeRawPointer? = nil
        var c_param_size: CUnsignedLong = 0
        switch contentType {
        case .json:
            if let `param` = param, let data = param.toData() {
                c_param = data.withUnsafePointer { unsafePointer -> UnsafeRawPointer in
                    UnsafeRawPointer(unsafePointer)
                }
                c_param_size = CUnsignedLong(data.count)
            }
        case .formURLEncoded:
            if let `param` = param, let data = param.queryString.data(using: .utf8) {
                c_param = data.withUnsafePointer { unsafePointer -> UnsafeRawPointer in
                    UnsafeRawPointer(unsafePointer)
                }
                c_param_size = CUnsignedLong(data.count)
            }
        case .propertyList:
            if let `param` = param, let data = param.toData() {
                c_param = data.withUnsafePointer { unsafePointer -> UnsafeRawPointer in
                    UnsafeRawPointer(unsafePointer)
                }
                c_param_size = CUnsignedLong(data.count)
            }
        }

        c_gtr_post(&taskID, url.cString(using: .utf8), cHeaders, timeOut, speedLimit, c_param, c_param_size)

        self.rwLock.withWriterLock { () -> Void in
            self.succeedContainer[taskID] = succeed
            self.failureContainer[taskID] = failure
        }

        return taskID
    }

    private func putRequest(url: String,
                            headers: [String: Encodable]? = nil,
                            contentType: ContentType = .json,
                            timeOut: UInt32,
                            speedLimit: Int,
                            param: [String: Any]? = nil,
                            succeed: GTRSucceedClosure?,
                            failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)

        var c_param: UnsafeRawPointer? = nil
        var c_param_size: CUnsignedLong = 0
        if let `param` = param, let data = param.toData() {
            c_param = data.withUnsafePointer { unsafePointer -> UnsafeRawPointer in
                UnsafeRawPointer(unsafePointer)
            }
            c_param_size = CUnsignedLong(data.count)
        }
        c_gtr_put(&taskID, url.cString(using: .utf8), cHeaders, timeOut, speedLimit, c_param, c_param_size)

        self.rwLock.withWriterLock { () -> Void in
            self.succeedContainer[taskID] = succeed
            self.failureContainer[taskID] = failure
        }

        return taskID
    }

    private func downloadRequest(url: String,
                                 filePath: String,
                                 headers: [String: Encodable]? = nil,
                                 contentType: ContentType = .json,
                                 timeOut: UInt32,
                                 speedLimit: Int,
                                 progress: GTRProgressClosure?,
                                 succeed: GTRSucceedClosure?,
                                 failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)
        c_gtr_download(&taskID, url.cString(using: .utf8), filePath.cString(using: .utf8), cHeaders, timeOut, speedLimit)
        return taskID
    }

    ///构建Headers
    private func generateHeaderString(headers: [String: Encodable]? = nil) -> [CChar]? {
        var allHeaders = self.httpHeaderClosure?() ?? [String: Encodable]()
        if let additionHeader = headers {
            allHeaders.merge(additionHeader) { (value1: Encodable, value2: Encodable) -> Encodable in
                value2
            }
        }
        let headerString = allHeaders.jsonStringEncoded()?.replacingOccurrences(of: "\\/", with: "/")
        let string = headerString?.cString(using: .utf8)

        return string
    }

    // MARK: 发送通知
    private func notifyRequestStatus(status: Notification.RequestStatusType,
                                     taskID: UInt32,
                                     method: Method) {
        let center = NotificationCenter.default
        let notificationName = status.notificationName
        let userInfo: [String: Any] = [
            Notification.userInfoTaskIDKey: taskID,
            Notification.userInfoMethodKey: method
        ]
        center.post(name: notificationName, object: nil, userInfo: userInfo)
    }
}

// MARK: - C Bridge
@_silgen_name("gtr_init")
private func c_gtr_init(_ user_agent: UnsafePointer<Int8>?, _ cylinder_count: UInt32)

@_silgen_name("gtr_get")
private func c_gtr_get(_ task_id: UnsafeMutablePointer<UInt32>?, _ url: UnsafePointer<Int8>?,
                       _ headers: UnsafePointer<Int8>?, _ time_out: UInt32, _ speed_limit: Int)

@_silgen_name("gtr_post")
private func c_gtr_post(_ task_id: UnsafeMutablePointer<UInt32>?, _ url: UnsafePointer<Int8>?,
                        _ headers: UnsafePointer<Int8>?, _ time_out: UInt32, _ speed_limit: Int,
                        _ param_data: UnsafeRawPointer?, _ param_size: UInt)

@_silgen_name("gtr_put")
private func c_gtr_put(_ task_id: UnsafeMutablePointer<UInt32>?, _ url: UnsafePointer<Int8>?,
                       _ headers: UnsafePointer<Int8>?, _ time_out: UInt32, _ speed_limit: Int,
                       _ param_data: UnsafeRawPointer?, _ param_size: UInt)

@_silgen_name("gtr_download")
private func c_gtr_download(_ task_id: UnsafeMutablePointer<UInt32>?, _ url: UnsafePointer<Int8>?,
                            _ filePath: UnsafePointer<Int8>?, _ headers: UnsafePointer<Int8>?,
                            _ time_out: UInt32, _ speed_limit: Int)

// MARK: - C CallBack

@_silgen_name("swift_get_request_succeed")
func c_get_request_succeed(task_id: CUnsignedInt,
                           c_header_data: UnsafeRawPointer,
                           c_header_data_size: CUnsignedLong,
                           c_body_data: UnsafeRawPointer,
                           c_body_data_size: CUnsignedLong) {
    let headerData = Data(bytes: c_header_data, count: Int(c_header_data_size))
    let header = handleHeader(headerData: headerData)

    let bodyData = Data(bytes: c_body_data, count: Int(c_body_data_size))

    __engine.rwLock.lockRead()
    let succeed = __engine.succeedContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        succeed??(header, bodyData)
    }
}

@_silgen_name("swift_get_request_failure")
func c_get_request_failure(task_id: CUnsignedInt,
                           http_response_code: CLong,
                           error_code: CInt,
                           error_message: UnsafePointer<CChar>) {
    __engine.rwLock.lockRead()
    let failure = __engine.failureContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
    }
}

@_silgen_name("swift_post_request_succeed")
func c_post_request_succeed(task_id: CUnsignedInt,
                            c_header_data: UnsafeRawPointer,
                            c_header_data_size: CUnsignedLong,
                            c_body_data: UnsafeRawPointer,
                            c_body_data_size: CUnsignedLong) {
    let headerData = Data(bytes: c_header_data, count: Int(c_header_data_size))
    let header = handleHeader(headerData: headerData)

    let bodyData = Data(bytes: c_body_data, count: Int(c_body_data_size))

    __engine.rwLock.lockRead()
    let succeed = __engine.succeedContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        succeed??(header, bodyData)
    }
}

@_silgen_name("swift_post_request_failure")
func c_post_request_failure(task_id: CUnsignedInt,
                            http_response_code: CLong,
                            error_code: CInt,
                            error_message: UnsafePointer<CChar>) {
    __engine.rwLock.lockRead()
    let failure = __engine.failureContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
    }
}

@_silgen_name("swift_put_request_succeed")
func c_put_request_succeed(task_id: CUnsignedInt,
                           c_header_data: UnsafeRawPointer,
                           c_header_data_size: CUnsignedLong,
                           c_body_data: UnsafeRawPointer,
                           c_body_data_size: CUnsignedLong) {
    let headerData = Data(bytes: c_header_data, count: Int(c_header_data_size))
    let header = handleHeader(headerData: headerData)

    let bodyData = Data(bytes: c_body_data, count: Int(c_body_data_size))

    __engine.rwLock.lockRead()
    let succeed = __engine.succeedContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        succeed??(header, bodyData)
    }
}

@_silgen_name("swift_put_request_failure")
func c_put_request_failure(task_id: CUnsignedInt,
                           http_response_code: CLong,
                           error_code: CInt,
                           error_message: UnsafePointer<CChar>) {
    __engine.rwLock.lockRead()
    let failure = __engine.failureContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
    }
}

@_silgen_name("swift_download_progress")
func c_download_progress(task_id: CUnsignedInt, download_now: CUnsignedLongLong, download_total: CUnsignedLongLong) {
    __engine.rwLock.lockRead()
    let progress = __engine.downloadProgressContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanDownloadProgress(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        progress??(download_now, download_total)
    }
}

@_silgen_name("swift_download_request_succeed")
func c_download_request_succeed(task_id: CUnsignedInt,
                                c_data: UnsafeRawPointer,
                                c_data_size: CUnsignedLong) {
    let swiftData = Data(bytes: c_data, count: Int(c_data_size))

    __engine.rwLock.lockRead()
    let succeed = __engine.succeedContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        //todo header
        succeed??(nil, swiftData)
    }
}

@_silgen_name("swift_download_request_failure")
func c_download_request_failure(task_id: CUnsignedInt,
                                http_response_code: CLong,
                                error_code: CInt,
                                error_message: UnsafePointer<CChar>) {
    __engine.rwLock.lockRead()
    let failure = __engine.failureContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
    }
}

func handleHeader(headerData: Data) -> HttpHeader? {
    let string = String(data: headerData, encoding: .utf8) ?? ""
    let list = string.components(separatedBy: "\n").map { (string: String) -> String in string.trimmingCharacters(in: CharacterSet.whitespacesAndNewlines) }
    let dictList = list.compactMap { (string: String) -> [String: String]? in
        let components = string.components(separatedBy: ": ").map { (string: String) -> String in string.trimmingCharacters(in: CharacterSet.whitespacesAndNewlines) }
        guard components.count > 1, let first = components.first, let last = components.last else { return nil }
        return [first: last]
    }
    let toJsonString = dictList.reduce("{") { (result: String, dictionary: [String: String]) -> String in
        guard let key = dictionary.keys.first, let value = dictionary.values.first else { return result }
        return result + "\"" + key + "\"" + ":" + "\"" + value + "\"" + ","
    }
    let result = toJsonString.dropLast().appending("}")
    guard let resultData = result.data(using: .utf8) else { return nil }
    let header = try? __jsonDecoder.decode(HttpHeader.self, from: resultData)
    return header
}

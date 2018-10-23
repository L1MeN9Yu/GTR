//
// Created L1MeN9Yu on 2018/5/16.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

typealias GTRSucceedClosure = (_ responseData: Foundation.Data) -> Void
typealias GTRFailureClosure = (_ httpResponseCode: Swift.Int, _ errorCode: Swift.Int32, _ errorMessage: Swift.String) -> Void
typealias GTRProgressClosure = (_ now: UInt64, _ total: UInt64) -> Void
typealias GTRHttpHeaderClosure = () -> [Swift.String: Swift.String]

extension GTR {
    class Engine {
        private(set) var responseQueue = DispatchQueue.main
        private(set) var succeedContainer: [Swift.UInt32: GTRSucceedClosure?] = [Swift.UInt32: GTRSucceedClosure?]()
        private(set) var failureContainer: [Swift.UInt32: GTRFailureClosure?] = [Swift.UInt32: GTRFailureClosure?]()
        private(set) var downloadProgressContainer: [Swift.UInt32: GTRProgressClosure?] = [Swift.UInt32: GTRProgressClosure?]()
        private(set) var uploadProgressContainer: [Swift.UInt32: GTRProgressClosure?] = [Swift.UInt32: GTRProgressClosure?]()

        private var httpHeaderClosure: GTRHttpHeaderClosure?
    }
}

// MARK: - Internal
extension GTR.Engine {
    func fire(engineNumber: String? = nil) {
        gtr_init(engineNumber?.cString(using: .utf8))
    }
}

// MARK: - Public
extension GTR.Engine {
    // MARK: Request
    @discardableResult
    public func request(httpMethod: GTR.Method,
                        url: String,
                        headers: [String: Encodable]? = nil,
                        contentType: GTR.ContentType = .json,
                        timeOut: UInt32,
                        param: [String: Any]? = nil,
                        succeed: GTRSucceedClosure?,
                        failure: GTRFailureClosure?) -> UInt32 {
        switch httpMethod {
        case .get:
            return self.getRequest(url: url, headers: headers, contentType: contentType, timeOut: timeOut, succeed: succeed, failure: failure)
        case .post:
            return self.postRequest(url: url, headers: headers, contentType: contentType, timeOut: timeOut, param: param, succeed: succeed, failure: failure)
        case .put:
            return self.putRequest(url: url, headers: headers, contentType: contentType, timeOut: timeOut, param: param, succeed: succeed, failure: failure)
        case .download:
            //TODO
            return 0
        case .upload:
            //TODO
            return 0
        }
    }

    // MARK: Config
    public func config(httpHeaderClosure: GTRHttpHeaderClosure?) {
        self.httpHeaderClosure = httpHeaderClosure
    }

    public func config(responseQueue: DispatchQueue) {
        self.responseQueue = responseQueue
    }
}

// MARK: - FilePrivate
extension GTR.Engine {
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
extension GTR.Engine {
    private func getRequest(url: String,
                            headers: [String: Encodable]? = nil,
                            contentType: GTR.ContentType = .json,
                            timeOut: UInt32,
                            succeed: GTRSucceedClosure?,
                            failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)

        gtr_get(&taskID, url.cString(using: .utf8), cHeaders, timeOut)
        self.succeedContainer[taskID] = succeed
        self.failureContainer[taskID] = failure

        return taskID
    }

    private func postRequest(url: String,
                             headers: [String: Encodable]? = nil,
                             contentType: GTR.ContentType = .json,
                             timeOut: UInt32,
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
                c_param = data.withUnsafeBytes { (unsafePointer: UnsafePointer<UInt8>) in
                    return UnsafeRawPointer(unsafePointer)
                }
                c_param_size = CUnsignedLong(data.count)
            }
        case .formURLEncoded:
            if let `param` = param, let data = param.queryString.data(using: .utf8) {
                c_param = data.withUnsafeBytes { (unsafePointer: UnsafePointer<UInt8>) in
                    return UnsafeRawPointer(unsafePointer)
                }
                c_param_size = CUnsignedLong(data.count)
            }
        case .propertyList:
            if let `param` = param, let data = param.toData() {
                c_param = data.withUnsafeBytes { (unsafePointer: UnsafePointer<UInt8>) in
                    return UnsafeRawPointer(unsafePointer)
                }
                c_param_size = CUnsignedLong(data.count)
            }
        }

        gtr_post(&taskID, url.cString(using: .utf8),
                cHeaders, timeOut, c_param, c_param_size)
        self.succeedContainer[taskID] = succeed
        self.failureContainer[taskID] = failure
        return taskID
    }

    private func putRequest(url: String,
                            headers: [String: Encodable]? = nil,
                            contentType: GTR.ContentType = .json,
                            timeOut: UInt32,
                            param: [String: Any]? = nil,
                            succeed: GTRSucceedClosure?,
                            failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)

        var c_param: UnsafeRawPointer? = nil
        var c_param_size: CUnsignedLong = 0
        if let `param` = param, let data = param.toData() {
            c_param = data.withUnsafeBytes { (unsafePointer: UnsafePointer<UInt8>) in
                return UnsafeRawPointer(unsafePointer)
            }
            c_param_size = CUnsignedLong(data.count)
        }
        gtr_put(&taskID, url.cString(using: .utf8), cHeaders, timeOut, c_param, c_param_size)
        self.succeedContainer[taskID] = succeed
        self.failureContainer[taskID] = failure

        return taskID
    }

    private func downloadRequest(url: String,
                                 filePath: String,
                                 headers: [String: Encodable]? = nil,
                                 contentType: GTR.ContentType = .json,
                                 timeOut: UInt32,
                                 progress: GTRProgressClosure?,
                                 succeed: GTRSucceedClosure?,
                                 failure: GTRFailureClosure?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let cHeaders: [CChar]? = self.generateHeaderString(headers: headers)
        gtr_download(&taskID, url.cString(using: .utf8), filePath.cString(using: .utf8), cHeaders, timeOut)
        return taskID
    }

    ///构建Headers
    private func generateHeaderString(headers: [String: Encodable]? = nil) -> [CChar]? {
        var allHeaders = self.httpHeaderClosure?() ?? [String: Encodable]()
        if let additionHeader = headers {
            allHeaders.merge(additionHeader) { (value1: Encodable, value2: Encodable) -> Encodable in
                return value2
            }
        }
        let headerString = allHeaders.jsonStringEncoded()?.replacingOccurrences(of: "\\/", with: "/")
        let string = headerString?.cString(using: .utf8)

        return string
    }
}

@_silgen_name("swift_get_request_succeed")
func c_get_request_succeed(task_id: CUnsignedInt,
                           c_data: UnsafeRawPointer,
                           c_data_size: CUnsignedLong) {
    let swiftData = Data.init(bytes: c_data, count: Int(c_data_size))
    let succeed = GTR.engine.succeedContainer[task_id]
    GTR.engine.responseQueue.async {
        succeed??(swiftData)
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_get_request_failure")
func c_get_request_failure(task_id: CUnsignedInt,
                           http_response_code: CLong,
                           error_code: CInt,
                           error_message: UnsafePointer<CChar>) {
    let failure = GTR.engine.failureContainer[task_id]
    GTR.engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_post_request_succeed")
func c_post_request_succeed(task_id: CUnsignedInt,
                            c_data: UnsafeRawPointer,
                            c_data_size: CUnsignedLong) {
    let swiftData = Data(bytes: c_data, count: Int(c_data_size))
    let succeed = GTR.engine.succeedContainer[task_id]
    GTR.engine.responseQueue.async {
        succeed??(swiftData)
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_post_request_failure")
func c_post_request_failure(task_id: CUnsignedInt,
                            http_response_code: CLong,
                            error_code: CInt,
                            error_message: UnsafePointer<CChar>) {
    let failure = GTR.engine.failureContainer[task_id]
    GTR.engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_put_request_succeed")
func c_put_request_succeed(task_id: CUnsignedInt,
                           c_data: UnsafeRawPointer,
                           c_data_size: CUnsignedLong) {
    let swiftData = Data(bytes: c_data, count: Int(c_data_size))
    let succeed = GTR.engine.succeedContainer[task_id]
    GTR.engine.responseQueue.async {
        succeed??(swiftData)
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_put_request_failure")
func c_put_request_failure(task_id: CUnsignedInt,
                           http_response_code: CLong,
                           error_code: CInt,
                           error_message: UnsafePointer<CChar>) {
    let failure = GTR.engine.failureContainer[task_id]
    GTR.engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_download_progress")
func c_download_progress(task_id: CUnsignedInt, download_now: CUnsignedLongLong, download_total: CUnsignedLongLong) {
    let progress = GTR.engine.downloadProgressContainer[task_id]
    GTR.engine.responseQueue.async {
        progress??(download_now, download_total)
    }
}

@_silgen_name("swift_download_request_succeed")
func c_download_request_succeed(task_id: CUnsignedInt,
                                c_data: UnsafeRawPointer,
                                c_data_size: CUnsignedLong) {
    let swiftData = Data(bytes: c_data, count: Int(c_data_size))
    let succeed = GTR.engine.succeedContainer[task_id]
    GTR.engine.responseQueue.async {
        succeed??(swiftData)
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}

@_silgen_name("swift_download_request_failure")
func c_download_request_failure(task_id: CUnsignedInt,
                                http_response_code: CLong,
                                error_code: CInt,
                                error_message: UnsafePointer<CChar>) {
    let failure = GTR.engine.failureContainer[task_id]
    GTR.engine.responseQueue.async {
        failure??(http_response_code, error_code, String(cString: error_message, encoding: .utf8) ?? "empty error message")
        GTR.engine.cleanResponseHandler(taskID: task_id)
    }
}
//
// Created L1MeN9Yu on 2018/5/16.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

typealias GTRProgressClosure = (_ now: UInt64, _ total: UInt64) -> Void
typealias GTRHttpHeaderClosure = () -> [Swift.String: Swift.String]

final class Engine {
    private init() {}
}

// MARK: - Variable
extension Engine {
    private(set) static var responseQueue = DispatchQueue.main
    private(set) static var notificationQueue = DispatchQueue.main
    private(set) static var completionContainer = [Swift.CUnsignedInt: GTR.Result?]()
    private(set) static var downloadProgressContainer: [Swift.UInt32: GTRProgressClosure?] = [Swift.UInt32: GTRProgressClosure?]()
    private(set) static var uploadProgressContainer: [Swift.UInt32: GTRProgressClosure?] = [Swift.UInt32: GTRProgressClosure?]()

    private static var httpHeaderClosure: GTRHttpHeaderClosure?

    fileprivate static let rwLock = ReadWriteLock()
}

// MARK: - Fire
extension Engine {
    internal static
    func fire(engineNumber: String? = nil, cylinderCount: UInt32) {
        c_gtr_init(engineNumber?.cString(using: .utf8), cylinderCount)
    }
}

// MARK: - Config

extension Engine {
    internal static
    func config(httpHeaderClosure: GTRHttpHeaderClosure?) {
        self.httpHeaderClosure = httpHeaderClosure
    }

    internal static
    func config(responseQueue: DispatchQueue) {
        self.responseQueue = responseQueue
    }
}

// MARK: - Request
extension Engine {
    internal static
    func getRequest(url: String,
                    headers: [String: CustomStringConvertible]? = nil,
                    method: Method,
                    contentType: ContentType = .json,
                    options: Option.Race,
                    responseInfoOption: Option.ResponseInfo,
                    speedLimit: Option.SpeedLimit,
                    proxy: Option.Proxy?,
                    param: [String: Any]? = nil,
                    completion: GTR.Result?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let getURL = urlForGetParameter(url: url, param: param)
        let dataTask = gtr_data_task_create(&taskID, getURL)
        config(dataTask: dataTask, headers: headers)
        gtr_data_task_config_parameters(dataTask, method.stringValue, nil, 0)
        gtr_data_task_config_options(dataTask, options.isDebug, options.timeout, options.maxRedirects)
        gtr_data_task_config_response_info_options(
                dataTask, responseInfoOption.base, responseInfoOption.time, responseInfoOption.size,
                responseInfoOption.speed, responseInfoOption.ssl, responseInfoOption.socket, responseInfoOption.cookie
        )
        gtr_data_task_config_speed(dataTask, speedLimit.maxReceiveSpeed, speedLimit.maxSendSpeed, speedLimit.lowSpeedLimit, speedLimit.lowSpeedTime)
        config(proxy: proxy, to: dataTask)
        gtr_data_task_start(dataTask)

        self.rwLock.withWriterLock { () -> Void in
            self.completionContainer[taskID] = completion
        }

        return taskID
    }

    internal static
    func postRequest(url: String,
                     headers: [String: CustomStringConvertible]? = nil,
                     method: Method,
                     contentType: ContentType = .json,
                     options: Option.Race,
                     responseInfoOption: Option.ResponseInfo,
                     speedLimit: Option.SpeedLimit,
                     proxy: Option.Proxy?,
                     param: [String: Any]? = nil,
                     completion: GTR.Result?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let dataTask = gtr_data_task_create(&taskID, url)
        let parameter = cParameter(dataTask: dataTask, contentType: contentType, param: param)
        config(dataTask: dataTask, headers: headers)
        gtr_data_task_config_parameters(dataTask, method.stringValue, parameter.0, parameter.1)
        gtr_data_task_config_options(dataTask, options.isDebug, options.timeout, options.maxRedirects)
        gtr_data_task_config_response_info_options(
                dataTask, responseInfoOption.base, responseInfoOption.time, responseInfoOption.size,
                responseInfoOption.speed, responseInfoOption.ssl, responseInfoOption.socket, responseInfoOption.cookie
        )
        gtr_data_task_config_speed(dataTask, speedLimit.maxReceiveSpeed, speedLimit.maxSendSpeed, speedLimit.lowSpeedLimit, speedLimit.lowSpeedTime)
        config(proxy: proxy, to: dataTask)
        gtr_data_task_start(dataTask)

        self.rwLock.withWriterLock { () -> Void in
            self.completionContainer[taskID] = completion
        }

        return taskID
    }

    internal static
    func customRequest(url: String,
                       headers: [String: CustomStringConvertible]? = nil,
                       method: Method,
                       contentType: ContentType = .json,
                       options: Option.Race,
                       responseInfoOption: Option.ResponseInfo,
                       speedLimit: Option.SpeedLimit,
                       proxy: Option.Proxy?,
                       param: [String: Any]? = nil,
                       completion: GTR.Result?) -> UInt32 {
        var taskID: CUnsignedInt = 0
        let dataTask = gtr_data_task_create(&taskID, url)
        let parameter = cParameter(dataTask: dataTask, contentType: contentType, param: param)
        config(dataTask: dataTask, headers: headers)
        gtr_data_task_config_parameters(dataTask, method.stringValue, parameter.0, parameter.1)
        gtr_data_task_config_options(dataTask, options.isDebug, options.timeout, options.maxRedirects)
        gtr_data_task_config_response_info_options(
                dataTask, responseInfoOption.base, responseInfoOption.time, responseInfoOption.size,
                responseInfoOption.speed, responseInfoOption.ssl, responseInfoOption.socket, responseInfoOption.cookie
        )
        gtr_data_task_config_speed(dataTask, speedLimit.maxReceiveSpeed, speedLimit.maxSendSpeed, speedLimit.lowSpeedLimit, speedLimit.lowSpeedTime)
        config(proxy: proxy, to: dataTask)
        gtr_data_task_start(dataTask)

        self.rwLock.withWriterLock { () -> Void in
            self.completionContainer[taskID] = completion
        }

        return taskID
    }

    internal static
    func downloadRequest(url: String,
                         filePath: String,
                         headers: [String: CustomStringConvertible]? = nil,
                         contentType: ContentType = .json,
                         timeOut: UInt32,
                         speedLimit: Int,
                         progress: GTRProgressClosure?,
                         completion: GTR.Result?) -> UInt32 {
        fatalError("not implement")
    }
}

// MARK: - Private
extension Engine {
    private static
    func urlForGetParameter(url: String, param: [String: Any]?) -> String {
        var _url = url

        if let queueParam = param?.filterValue(valueType: String.self),
           let url = URL(string: url)?.appendingQueryParameters(queueParam) {
            _url = url.absoluteString
        }

        return _url
    }

    private static
    func cParameter(dataTask: OpaquePointer, contentType: ContentType, param: [String: Any]?) -> (UnsafeRawPointer?, CUnsignedLong) {
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
        case .formData:
            if let param = param,
               let formData = param[ContentType.FormDataKey] as? [Mime] {
                formData.forEach { (mime: Mime) in
                    switch mime {
                    case .text(let name, let value):
                        gtr_data_task_add_form_data(dataTask, mime.type, name.cString(using: .utf8), value.cString(using: .utf8))
                    case .file(let name, let fileURL):
                        gtr_data_task_add_form_data(dataTask, mime.type, name.cString(using: .utf8), fileURL.path.cString(using: .utf8))
                    }
                }
            }
            break
        }
        return (c_param, c_param_size)
    }

    ///构建Headers
    private static
    func config(dataTask: OpaquePointer, headers: [String: CustomStringConvertible]? = nil) {
        var allHeaders = self.httpHeaderClosure?() ?? [String: CustomStringConvertible]()
        if let additionHeader = headers {
            allHeaders.merge(additionHeader) { (value1: CustomStringConvertible, value2: CustomStringConvertible) -> CustomStringConvertible in value2 }
        }
        allHeaders.forEach { key, value in
            gtr_data_task_add_header(dataTask, "\(key): \(value)")
        }
    }

    private static
    func config(proxy: Option.Proxy?, to dataTask: OpaquePointer) {
        guard let proxy = proxy else { return }
        gtr_data_task_config_proxy(dataTask, proxy.url.cString(using: .utf8), proxy.port)
    }
}

// MARK: - FilePrivate
extension Engine {
    fileprivate static
    func cleanResponseHandler(taskID: UInt32) {
        self.completionContainer.removeValue(forKey: taskID)
    }

    fileprivate static
    func cleanDownloadProgress(taskID: UInt32) {
        self.downloadProgressContainer.removeValue(forKey: taskID)
    }

    fileprivate static
    func cleanUploadProgress(taskID: UInt32) {
        self.uploadProgressContainer.removeValue(forKey: taskID)
    }
}

extension Engine {
    internal static
    func dataTaskSucceed(
            task_id: UInt32,
            response_info_data: UnsafeRawPointer, response_info_data_size: Int,
            c_header_data: UnsafeRawPointer, c_header_data_size: Int,
            c_body_data: UnsafeRawPointer, c_body_data_size: Int
    ) {
        let headerData = Data(bytes: c_header_data, count: c_header_data_size)
        let header = handleHeader(headerData: headerData)

        let bodyData = Data(bytes: c_body_data, count: c_body_data_size)

        let cResponseData = Data(bytes: response_info_data, count: response_info_data_size)
        let responseInfo: ResponseInfo? = Coder.decode(data: cResponseData)

        rwLock.lockRead()
        let result = completionContainer[task_id]
        rwLock.unlock()

        rwLock.lockWrite()
        cleanResponseHandler(taskID: task_id)
        rwLock.unlock()

        responseQueue.async {
            result??(Destination.success(Goal(header: header, body: bodyData, info: responseInfo)))
        }
    }

    internal static
    func dataTaskFailed(
            task_id: UInt32,
            response_info_data: UnsafeRawPointer, response_info_data_size: Int,
            error_code: Int32, error_message: UnsafePointer<Int8>
    ) {
        let cResponseData = Data(bytes: response_info_data, count: response_info_data_size)
        let responseInfo: ResponseInfo? = Coder.decode(data: cResponseData)

        rwLock.lockRead()
        let result = completionContainer[task_id]
        rwLock.unlock()

        rwLock.lockWrite()
        cleanResponseHandler(taskID: task_id)
        rwLock.unlock()

        responseQueue.async {
            //ToDo [L1MeN9Yu]
            result??(Destination.failure(RaceError(responseInfo: responseInfo, errorCode: error_code, errorMessage: String(cString: error_message, encoding: .utf8) ?? "")))
        }
    }
}

// MARK: - C CallBack

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
    //ToDo [L1MeN9Yu] implement
    fatalError("not implement")

/*    let swiftData = Data(bytes: c_data, count: Int(c_data_size))

    __engine.rwLock.lockRead()
    let result = __engine.completionContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        result??(Destination.success(Goal(header: nil, body: swiftData)))
    }*/
}

@_silgen_name("swift_download_request_failure")
func c_download_request_failure(task_id: CUnsignedInt,
                                http_response_code: CLong,
                                error_code: CInt,
                                error_message: UnsafePointer<CChar>) {
    //ToDo [L1MeN9Yu] implement
    fatalError("not implement")
/*    __engine.rwLock.lockRead()
    let result = __engine.completionContainer[task_id]
    __engine.rwLock.unlock()

    __engine.rwLock.lockWrite()
    __engine.cleanResponseHandler(taskID: task_id)
    __engine.rwLock.unlock()

    __engine.responseQueue.async {
        result??(Destination.failure(RaceError(httpResponseCode: http_response_code, errorCode: error_code, errorMessage: String(cString: error_message, encoding: .utf8) ?? "")))
    }*/
}

func handleHeader(headerData: Data) -> Data? {
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
    return resultData
}

//
// Created L1MeN9Yu on 2018/5/23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

private(set) var __gearbox = { () -> Gearbox in
    let gearbox = Gearbox()
    return gearbox
}()

private(set) var __engine = { () -> Engine in
    let engine = Engine()
    return engine
}()

private(set) var __hud = { () -> HUD in
    let hud = HUD()
    return hud
}()

private var __driver: Driver.Type?

private var __horn: Horn.Type?

@discardableResult
func request(method: Method = .get,
             url: String,
             contentType: ContentType = .json,
             headers: [String: Encodable]? = nil,
             timeOut: UInt32,
             speedLimit: Int,
             param: [String: Any]? = nil,
             downloadPath: String? = nil,
             progress: ((_ now: UInt64, _ total: UInt64) -> Void)? = nil,
             complete: Result?) -> UInt32 {
    var allHeaders = contentType.toHeader()

    if let globalHeader = __driver?.identity() {
        allHeaders.merge(globalHeader) { (value_old: Encodable, value_new: Encodable) -> Encodable in value_new }
    }

    if let h = headers {
        allHeaders.merge(h) { (value_old: Encodable, value_new: Encodable) -> Encodable in value_new }
    }

    return __engine.request(
            httpMethod: method,
            url: url,
            headers: allHeaders,
            contentType: contentType,
            timeOut: timeOut,
            speedLimit: speedLimit,
            param: param,
            downloadPath: downloadPath,
            progress: progress,
            succeed: { (header, data) in
                complete?(Destination.win(httpHeader: header, responseData: data))
            },
            failure: { (httpResponseCode, errorCode, errorMessage) in
                complete?(Destination.lose(httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage))
                notifyFailure(url: url, headers: allHeaders, contentType: contentType, param: param, httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage)
            })
}


// MARK: - Config
public func setup(driver: Driver.Type? = nil, horn: Horn.Type? = nil, cylinderCount: UInt32 = 8) {
    __driver = driver
    __horn = horn
    __engine.fire(engineNumber: driver?.userAgent(), cylinderCount: cylinderCount)
    __gearbox.start()
}

public func config(responseQueue: DispatchQueue?) {
    guard let q = responseQueue else {
        __engine.config(responseQueue: DispatchQueue.main)
        return
    }

    __engine.config(responseQueue: q)
}

public func configProxy(isEnable: Bool, url: String, port: UInt32) {
    __gearbox.config(proxy: isEnable, url: url, port: port)
}

public func fetchProxyInfo() -> (String, UInt32)? {
    if let proxyInfo = Gearbox.proxyInfo {
        return (proxyInfo.url, proxyInfo.port)
    }
    return nil
}

// MARK: - Extern
func notifyFailure(url: String, headers: [String: Encodable]?, contentType: ContentType, param: [String: Any]?,
                   httpResponseCode: Int, errorCode: Int32, errorMessage: String,
                   filename: String = #file, function: String = #function, line: Int = #line) {
    let message = "response code = \(httpResponseCode)\n errorCode = \(errorCode)\n errorMessage = \(errorMessage)\n"
    __horn?.whistle(type: .error, message: message, filename: filename, function: function, line: line)
    __horn?.raceDidLost(url: url, headers: headers, contentType: contentType, param: param, httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage)
}

public func whistle(type: HornType, message: String, filename: String = #file,
                    function: String = #function, line: Int = #line) {
    __horn?.whistle(type: type, message: message, filename: filename, function: function, line: line)
}

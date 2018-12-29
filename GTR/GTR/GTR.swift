//
// Created L1MeN9Yu on 2018/5/23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

private(set) var gearbox = { () -> GTR.Gearbox in
    let gearbox = GTR.Gearbox()
    return gearbox
}()

private(set) var engine = { () -> GTR.Engine in
    let engine = GTR.Engine()
    return engine
}()

private(set) var hud = { () -> GTR.HUD in
    let hud = GTR.HUD()
    return hud
}()

private var driver: Driver.Type?

private var horn: Horn.Type?

@discardableResult
func request(method: GTR.Method = .get,
             url: String,
             contentType: GTR.ContentType = .json,
             headers: [String: Encodable]? = nil,
             timeOut: UInt32,
             param: [String: Any]? = nil,
             downloadPath: String? = nil,
             progress: ((_ now: UInt64, _ total: UInt64) -> Void)? = nil,
             complete: Result?) -> UInt32 {
    var allHeaders = contentType.toHeader()

    if let globalHeader = GTR.driver?.identity() {
        allHeaders.merge(globalHeader) { (value_old: Encodable, value_new: Encodable) -> Encodable in
            return value_new
        }
    }

    if let h = headers {
        allHeaders.merge(h) { (value_old: Encodable, value_new: Encodable) -> Encodable in
            return value_new
        }
    }

    return GTR.engine.request(
            httpMethod: method,
            url: url,
            headers: allHeaders,
            contentType: contentType,
            timeOut: timeOut,
            param: param,
            downloadPath: downloadPath,
            progress: progress,
            succeed: { data in
                complete?(GTR.Destination.win(responseData: data))
            },
            failure: { (httpResponseCode, errorCode, errorMessage) in
                complete?(GTR.Destination.lose(httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage))
                GTR.notifyFailure(url: url, headers: allHeaders, contentType: contentType, param: param, httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage)
            })
}


// MARK: - Config
public func setup(driver: Driver.Type? = nil, horn: Horn.Type? = nil, cylinderCount: UInt32 = 8) {
    GTR.driver = driver
    GTR.horn = horn
    GTR.engine.fire(engineNumber: GTR.driver?.userAgent(), cylinderCount: cylinderCount)
    GTR.gearbox.start()
}

public func config(responseQueue: DispatchQueue?) {
    guard let q = responseQueue else {
        GTR.engine.config(responseQueue: DispatchQueue.main)
        return
    }

    GTR.engine.config(responseQueue: q)
}

public func configProxy(isEnable: Bool, url: String, port: UInt32) {
    GTR.gearbox.config(proxy: isEnable, url: url, port: port)
}

public func fetchProxyInfo() -> (String, UInt32)? {
    if let proxyInfo = Gearbox.proxyInfo {
        return (proxyInfo.url, proxyInfo.port)
    }
    return nil
}

// MARK: - Extern
func notifyFailure(url: String, headers: [String: Encodable]?, contentType: GTR.ContentType, param: [String: Any]?,
                   httpResponseCode: Int, errorCode: Int32, errorMessage: String,
                   filename: String = #file, function: String = #function, line: Int = #line) {
    let message = "response code = \(httpResponseCode)\n errorCode = \(errorCode)\n errorMessage = \(errorMessage)\n"
    GTR.horn?.whistle(type: .error, message: message, filename: filename, function: function, line: line)
    GTR.horn?.raceDidLost(url: url, headers: headers, contentType: contentType, param: param, httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage)
}

public func whistle(type: HornType, message: String, filename: String = #file,
                    function: String = #function, line: Int = #line) {
    GTR.horn?.whistle(type: type, message: message, filename: filename, function: function, line: line)
}

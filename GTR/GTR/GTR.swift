//
// Created L1MeN9Yu on 2018/5/23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

public class GTR {
    private(set) static var gearbox = { () -> GTR.Gearbox in
        let gearbox = GTR.Gearbox()
        return gearbox
    }()

    private(set) static var engine = { () -> GTR.Engine in
        let engine = GTR.Engine()
        return engine
    }()

    private(set) static var hud = { () -> GTR.HUD in
        let hud = GTR.HUD()
        return hud
    }()

    private static var driver: Driver.Type?

    private static var horn: Horn.Type?

    @discardableResult
    public class func race(method: GTR.Method = .get,
                           url: String,
                           contentType: GTR.ContentType = .json,
                           headers: [String: Encodable]? = nil,
                           timeOut: UInt32,
                           param: [String: Any]? = nil,
                           downloadPath: String? = nil,
                           progress: ((_ now: UInt64, _ total: UInt64) -> Void)? = nil,
                           complete: Complete?) -> UInt32 {
        var allHeaders = contentType.toHeader()

        if let globalHeader = self.driver?.identity() {
            allHeaders.merge(globalHeader) { (value_old: Encodable, value_new: Encodable) -> Encodable in
                return value_new
            }
        }

        if let h = headers {
            allHeaders.merge(h) { (value_old: Encodable, value_new: Encodable) -> Encodable in
                return value_new
            }
        }

        return self.engine.request(
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
                    self.notifyFailure(url: url, headers: allHeaders, contentType: contentType, param: param, httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage)
                })
    }
}

// MARK: - Config
extension GTR {
    public class func setup(driver: Driver.Type? = nil, horn: Horn.Type? = nil) {
        self.driver = driver
        self.horn = horn
        self.engine.fire(engineNumber: self.driver?.userAgent())
        self.gearbox.start()
    }

    public class func config(responseQueue: DispatchQueue?) {
        guard let q = responseQueue else {
            self.engine.config(responseQueue: DispatchQueue.main)
            return
        }

        self.engine.config(responseQueue: q)
    }

    public class func configProxy(isEnable: Bool, url: String, port: UInt32) {
        self.gearbox.config(proxy: isEnable, url: url, port: port)
    }

    public class func fetchProxyInfo() -> (String, UInt32)? {
        if let proxyInfo = self.proxyInfo {
            return (proxyInfo.url, proxyInfo.port)
        }
        return nil
    }
}

// MARK: - Extern
extension GTR {
    class func notifyFailure(url: String, headers: [String: Encodable]?, contentType: GTR.ContentType, param: [String: Any]?,
                             httpResponseCode: Int, errorCode: Int32, errorMessage: String,
                             filename: String = #file, function: String = #function, line: Int = #line) {
        let message = "response code = \(httpResponseCode)\n errorCode = \(errorCode)\n errorMessage = \(errorMessage)\n"
        self.horn?.whistle(type: .error, message: message, filename: filename, function: function, line: line)
    }

    @available(iOS, deprecated: 0.3.0, message: "func does noting now")
    public class func logLose(httpResponseCode: Int, errorCode: Int32, errorMessage: String,
                              filename: String = #file, function: String = #function, line: Int = #line) {
//        let message = "response code = \(httpResponseCode)\n errorCode = \(errorCode)\n errorMessage = \(errorMessage)\n"
//        self.horn?.whistle(type: .error, message: message, filename: filename, function: function, line: line)
    }

    public class func whistle(type: HornType, message: String, filename: String = #file,
                              function: String = #function, line: Int = #line) {
        self.horn?.whistle(type: type, message: message, filename: filename, function: function, line: line)
    }
}

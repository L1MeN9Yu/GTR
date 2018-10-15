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

    private static var driver: Driver?

    private static var horn: Horn?

    private static var hornType: Horn.Type?

    @discardableResult
    public class func race(method: GTR.Method = .get,
                           url: String,
                           contentType: GTR.ContentType = .json,
                           headers: [String: Encodable]? = nil,
                           timeOut: UInt32,
                           param: [String: Any]? = nil,
                           complete: @escaping GTR.Complete) -> UInt32 {
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
                succeed: { data in
                    complete(GTR.Destination.win(responseData: data))
                },
                failure: { (httpResponseCode, errorCode, errorMessage) in
                    complete(GTR.Destination.lose(httpResponseCode: httpResponseCode, errorCode: errorCode, errorMessage: errorMessage))
                })
    }
}

// MARK: - Config
extension GTR {
    public class func setup(driver: Driver? = nil, horn: Horn? = nil, hornType: Horn.Type? = nil) {
        self.driver = driver
        self.horn = horn
        self.hornType = hornType
        self.engine.fire(engineNumber: self.driver?.userAgent())
        self.gearbox.start()
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

// MARK: - Http Method
extension GTR {
    public enum Method: UInt {
        case get
        case post
        case put
    }
}

// MARK: - Content Type
extension GTR {
    public enum ContentType: UInt {
        case json
        case formURLEncoded
        case propertyList

        fileprivate func toHeader() -> [String: Encodable] {
            switch self {
            case .json:
                return ["Content-Type": "application/json; charset=utf-8"]
            case .formURLEncoded:
                return ["Content-Type": "application/x-www-form-urlencoded; charset=utf-8"]
            case .propertyList:
                return ["Content-Type": "application/x-plist; charset=utf-8"]
            }
        }
    }
}

// MARK: - Destination
extension GTR {
    public enum Destination {
        case win(responseData: Foundation.Data)
        case lose(httpResponseCode: Int, errorCode: Int32, errorMessage: String)
    }
}

// MARK: - Typealias
extension GTR {
    public typealias Complete = (GTR.Destination) -> Void
}

// MARK: - Extern
extension GTR {
    public class func logLose(httpResponseCode: Int, errorCode: Int32, errorMessage: String,
                              filename: String = #file, function: String = #function, line: Int = #line) {
        let message = "response code = \(httpResponseCode)\n errorCode = \(errorCode)\n errorMessage = \(errorMessage)\n"
        self.hornType?.whistle(message: message, filename: filename, function: function, line: line)
    }

    public class func whistle(message: String, filename: String = #file,
                              function: String = #function, line: Int = #line) {
        if let hornType = self.hornType {
            hornType.whistle(message: message, filename: filename, function: function, line: line)
        }
    }
}

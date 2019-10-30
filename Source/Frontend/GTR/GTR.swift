//
// Created L1MeN9Yu on 2018/5/23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

// MARK: - Components
private(set) var __gearbox = { () -> Gearbox in
    let gearbox = Gearbox()
    return gearbox
}()

private(set) var __engine = { () -> Engine in
    let engine = Engine()
    return engine
}()

private(set) var __driver: Driver.Type!

private(set) var __horn: Horn.Type!

private(set) var __optionalEquipments: OptionalEquipments.Type!

// MARK: - Tasks
@discardableResult
func dataTask(method: Method,
              url: String,
              contentType: ContentType,
              headers: [String: Encodable]?,
              options: RaceOptions,
              speedLimit: RaceSpeedLimit,
              param: [String: Any]?,
              completion: Result?) -> UInt32 {
    var allHeaders = contentType.toHeader()

    if let globalHeader = __driver?.identity() {
        allHeaders.merge(globalHeader) { (value_old: Encodable, value_new: Encodable) -> Encodable in value_new }
    }

    if let h = headers {
        allHeaders.merge(h) { (value_old: Encodable, value_new: Encodable) -> Encodable in value_new }
    }

    switch method {
    case .get:
        return __engine.getRequest(url: url, headers: allHeaders, method: method, contentType: contentType, options: options, speedLimit: speedLimit, param: param, completion: completion)
    case .post:
        return __engine.postRequest(url: url, headers: allHeaders,method: method,contentType: contentType, options: options, speedLimit: speedLimit, param: param, completion: completion)
    case .custom(_):
        return __engine.customRequest(url: url, headers: allHeaders, method: method, contentType: contentType, options: options, speedLimit: speedLimit, param: param, completion: completion)
//    case .download:
//        guard let downloadPath = downloadPath else { fatalError("must use download path") }
//        return __engine.downloadRequest(url: url, filePath: downloadPath, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, progress: progress, completion: completion)
//    case .upload:
//        //TODO
//        fatalError("not implement yet")
    }
}


// MARK: - Config
public func setup(driver: Driver.Type, horn: Horn.Type, optionalEquipments: OptionalEquipments.Type) {
    __driver = driver
    __horn = horn
    __optionalEquipments = optionalEquipments
    __engine.fire(engineNumber: driver.userAgent(), cylinderCount: __optionalEquipments.threadCount)
    __engine.config(responseQueue: __optionalEquipments.responseQueue)
    __gearbox.start()
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
func whistle(type: HornType, message: String, filename: String = #file,
             function: String = #function, line: Int = #line) {
    __horn?.whistle(type: type, message: message, filename: filename, function: function, line: line)
}

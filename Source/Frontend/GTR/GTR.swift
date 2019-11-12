//
// Created L1MeN9Yu on 2018/5/23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

// MARK: - Components

private(set) var __engine = { () -> Engine in
    let engine = Engine()
    return engine
}()

private(set) var __driver: Driver.Type!

private(set) var __horn: Horn.Type!

private(set) var __gearBox: GearBox.Type!

// MARK: - Tasks
@discardableResult
func dataTask(method: Method,
              url: String,
              contentType: ContentType,
              headers: [String: Encodable]?,
              options: RaceOptions,
              speedLimit: RaceSpeedLimit,
              proxy: (String, Int)?,
              param: [String: Any]?,
              completion: Result?) -> UInt32 {
    var allHeaders = contentType.toHeader()

    if let globalHeader = __driver.identity {
        allHeaders.merge(globalHeader) { (value_old: Encodable, value_new: Encodable) -> Encodable in value_new }
    }

    if let h = headers {
        allHeaders.merge(h) { (value_old: Encodable, value_new: Encodable) -> Encodable in value_new }
    }

    switch method {
    case .get:
        return __engine.getRequest(
                url: url, headers: allHeaders, method: method, contentType: contentType,
                options: options, speedLimit: speedLimit, proxy: proxy, param: param, completion: completion)
    case .post:
        return __engine.postRequest(url: url, headers: allHeaders, method: method, contentType: contentType,
                options: options, speedLimit: speedLimit, proxy: proxy, param: param, completion: completion)
    case .custom(_):
        return __engine.customRequest(url: url, headers: allHeaders, method: method, contentType: contentType,
                options: options, speedLimit: speedLimit, proxy: proxy, param: param, completion: completion)
//    case .download:
//        guard let downloadPath = downloadPath else { fatalError("must use download path") }
//        return __engine.downloadRequest(url: url, filePath: downloadPath, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, progress: progress, completion: completion)
//    case .upload:
//        //TODO
//        fatalError("not implement yet")
    }
}


// MARK: - Config
public func setup(agent: Agent, logger: Logger, configuration: Configuration) {
    __driver = agent
    __horn = logger
    __gearBox = configuration
    __engine.fire(engineNumber: agent.userAgent, cylinderCount: __gearBox.threadCount)
    __engine.config(responseQueue: __gearBox.responseQueue)
}

// MARK: - Extern
func whistle(type: HornType, message: String, filename: String = #file,
             function: String = #function, line: Int = #line) {
    __horn?.whistle(type: type, message: message, filename: filename, function: function, line: line)
}

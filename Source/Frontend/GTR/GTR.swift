//
// Created L1MeN9Yu on 2018/5/23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

// MARK: - Components

import Foundation

private(set) var __engine = Engine.self

private(set) var __driver: Driver.Type!

private(set) var __horn: Horn.Type!

private(set) var __gearBox: GearBox.Type!

private(set) var __fired = false

// MARK: - Tasks
@discardableResult
func dataTask(method: Method,
              url: String,
              contentType: ContentType,
              headers: [String: CustomStringConvertible]?,
              options: Option.Race,
              responseInfoOption: Option.ResponseInfo,
              speedLimit: Option.SpeedLimit,
              proxy: Option.Proxy?,
              param: [String: Any]?,
              completion: Result?) -> UInt32 {
//    precondition(fired, "must setup first")
//
//    var allHeaders = contentType.toHeader()
//
//    if let globalHeader = __driver.identity {
//        allHeaders.merge(globalHeader) { (value_old: CustomStringConvertible, value_new: CustomStringConvertible) -> CustomStringConvertible in value_new }
//    }
//
//    if let h = headers {
//        allHeaders.merge(h) { (value_old: CustomStringConvertible, value_new: CustomStringConvertible) -> CustomStringConvertible in value_new }
//    }
//
//    switch method {
//    case .get:
//        return __engine.getRequest(
//                url: url, headers: allHeaders, method: method, contentType: contentType,
//                options: options, responseInfoOption: responseInfoOption,
//                speedLimit: speedLimit, proxy: proxy, param: param, completion: completion
//        )
//    case .post:
//        return __engine.postRequest(
//                url: url, headers: allHeaders, method: method, contentType: contentType,
//                options: options, responseInfoOption: responseInfoOption,
//                speedLimit: speedLimit, proxy: proxy, param: param, completion: completion
//        )
//    case .custom(_):
//        return __engine.customRequest(
//                url: url, headers: allHeaders, method: method, contentType: contentType,
//                options: options, responseInfoOption: responseInfoOption,
//                speedLimit: speedLimit, proxy: proxy, param: param, completion: completion
//        )
//    case .download:
//        guard let downloadPath = downloadPath else { fatalError("must use download path") }
//        return __engine.downloadRequest(url: url, filePath: downloadPath, headers: headers, contentType: contentType, timeOut: timeOut, speedLimit: speedLimit, progress: progress, completion: completion)
//    case .upload:
//        //TODO
//        fatalError("not implement yet")
//    }
    return 0
}


// MARK: - Config
public func setup(agent: Agent, logger: Logger, configuration: Configuration) {
    __driver = agent
    __horn = logger
    __gearBox = configuration
    __engine.fire(engineNumber: agent.userAgent, cylinderCount: __gearBox.threadCount)
    __engine.config(responseQueue: __gearBox.responseQueue)
    __fired = true
}

// MARK: - Extern
func whistle(type: HornType, message: String, filename: String = #file,
             function: String = #function, line: Int = #line) {
    __horn?.whistle(type: type, message: message, filename: filename, function: function, line: line)
}

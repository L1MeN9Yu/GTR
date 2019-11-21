//
// Created L1MeN9Yu on 2018/7/12.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

public typealias Agent = Driver.Type

public protocol Driver {
    ///全局Header
    static var identity: [String: CustomStringConvertible]? { get }

    ///UserAgent
    static var userAgent: String? { get }
}

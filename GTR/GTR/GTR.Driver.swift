//
// Created L1MeN9Yu on 2018/7/12.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

public protocol Driver {
    ///全局Header
    static var identity: () -> [String: Encodable]? { get }

    ///UserAgent
    static var userAgent: () -> String? { get }
}

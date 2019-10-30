//
// Created by Mengyu Li on 2019/10/30.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public typealias TaskOptions = RaceOptions

public struct RaceOptions {
    let isDebug: Bool

    let timeout: UInt32

    public init(isDebug: Bool, timeout: UInt32) {
        self.isDebug = isDebug
        self.timeout = timeout
    }
}

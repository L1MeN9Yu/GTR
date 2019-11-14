//
// Created by Mengyu Li on 2019/11/14.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public struct Option {
    private init() {}
}

extension Option {
    static let defaultSpeedLimit = SpeedLimit(maxReceiveSpeed: 0, maxSendSpeed: 0, lowSpeedLimit: 0, lowSpeedTime: 0)
    static let defaultTask = Race(isDebug: __gearBox.debug, timeout: __gearBox.timeout)
    static let defaultResponseInfo = ResponseInfo(base: false, time: false, size: false, speed: false, ssl: false, socket: false, cookie: false)
}
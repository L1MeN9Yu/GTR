//
// Created by Mengyu Li on 2019/10/30.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public typealias TaskSpeedLimit = Option.SpeedLimit

public extension Option {
    struct SpeedLimit {
        public let maxReceiveSpeed: CLong
        public let maxSendSpeed: CLong
        public let lowSpeedLimit: CLong
        public let lowSpeedTime: CLong

        public init(maxReceiveSpeed: CLong, maxSendSpeed: CLong, lowSpeedLimit: CLong, lowSpeedTime: CLong) {
            self.maxReceiveSpeed = maxReceiveSpeed
            self.maxSendSpeed = maxSendSpeed
            self.lowSpeedLimit = lowSpeedLimit
            self.lowSpeedTime = lowSpeedTime
        }
    }
}

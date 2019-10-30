//
// Created by Mengyu Li on 2019/10/30.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public typealias TaskSpeedLimit = RaceSpeedLimit

public struct RaceSpeedLimit {
    let maxReceiveSpeed: CLong
    let maxSendSpeed: CLong
    let lowSpeedLimit: CLong
    let lowSpeedTime: CLong

    public init(maxReceiveSpeed: CLong = 0, maxSendSpeed: CLong = 0, lowSpeedLimit: CLong = 0, lowSpeedTime: CLong = 0) {
        self.maxReceiveSpeed = maxReceiveSpeed
        self.maxSendSpeed = maxSendSpeed
        self.lowSpeedLimit = lowSpeedLimit
        self.lowSpeedTime = lowSpeedTime
    }
}

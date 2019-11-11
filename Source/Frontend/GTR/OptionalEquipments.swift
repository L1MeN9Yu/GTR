//
// Created by Mengyu Li on 2019/10/30.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import class Dispatch.DispatchQueue

public typealias Configuration = OptionalEquipments

public protocol OptionalEquipments {
    static var timeout: UInt32 { get }
    static var responseQueue: DispatchQueue { get }
    static var debug: Bool { get }
    static var threadCount: UInt32 { get }
}

public extension OptionalEquipments {
    static var timeout: UInt32 { 10 }
    static var responseQueue: DispatchQueue { DispatchQueue.main }
    static var debug: Bool { false }
    static var threadCount: UInt32 { 8 }
}
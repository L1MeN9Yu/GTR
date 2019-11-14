//
// Created by Mengyu Li on 2019/10/30.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import class Dispatch.DispatchQueue
import class Foundation.ProcessInfo

public typealias Configuration = GearBox.Type

public protocol GearBox {
    /// timeout , default is 10
    static var timeout: UInt32 { get }
    /// responseQueue , default is main queue
    static var responseQueue: DispatchQueue { get }
    /// is debug mode , default is false
    static var debug: Bool { get }
    /// number of thread to use , default is ProcessInfo.processInfo.activeProcessorCount
    static var threadCount: UInt32 { get }
    /// proxy , default is nil
    static var proxy: Option.Proxy? { get }
    /// response info should fetch , default is don't get any info
    static var responseInfoOption: Option.ResponseInfo { get }
}

public extension GearBox {
    static var timeout: UInt32 { 10 }
    static var responseQueue: DispatchQueue { DispatchQueue.main }
    static var debug: Bool { false }
    static var threadCount: UInt32 { UInt32(ProcessInfo.processInfo.activeProcessorCount) }
    static var proxy: Option.Proxy? { nil }
    static var responseInfoOption: Option.ResponseInfo { Option.defaultResponseInfo }
}
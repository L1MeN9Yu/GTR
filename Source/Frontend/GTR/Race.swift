//
// Created by Mengyu Li on 2018/8/10.
// Copyright (c) 2018 top.limengyu. All rights reserved.
//

import Foundation

public typealias Race = DataTask

public protocol DataTask {
    var url: String { get }
    var timeout: UInt32 { get }
    var speedLimit: Int { get }
    var headers: [String: Encodable]? { get }
    var method: Method { get }
    var contentType: ContentType { get }
    var parameters: [String: Encodable]? { get }
}

extension Race {
    public var timeout: UInt32 { 10 }

    public var speedLimit: Int { 0 }

    public var headers: [String: Encodable]? { nil }

    public var method: Method { .get }

    public var contentType: ContentType { .json }

    public var parameters: [String: Encodable]? { nil }
}

extension Race {
    @discardableResult
    public func race(completion: @escaping Result) -> CUnsignedInt {
        dataTask(method: method,
                url: url,
                contentType: contentType,
                headers: headers,
                timeOut: timeout,
                speedLimit: speedLimit,
                param: parameters,
                completion: completion)
    }
}

//public protocol LongDistanceRace: Race {
//    var filePath: String { get }
//    var progress: ((_ now: UInt64, _ total: UInt64) -> Void)? { get }
//}
//
//extension LongDistanceRace {
//    public var progress: ((UInt64, UInt64) -> Void)? { nil }
//}
//
//public func longDistanceRace(race: LongDistanceRace, complete: @escaping (Result)) -> UInt32 {
//    assert((race.method == Method.download || race.method == Method.upload), "method must be download or upload")
//    return request(
//            method: race.method,
//            url: race.url,
//            contentType: race.contentType,
//            headers: race.headers,
//            timeOut: race.timeout,
//            speedLimit: race.speedLimit,
//            param: race.parameters,
//            downloadPath: race.filePath,
//            progress: race.progress,
//            completion: complete)
//}

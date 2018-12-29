//
// Created by Mengyu Li on 2018/8/10.
// Copyright (c) 2018 HuJiang. All rights reserved.
//

import Foundation

public protocol Race {
    var url: String { get }
    var timeout: UInt32 { get }
    var headers: [String: Encodable]? { get }
    var method: GTR.Method { get }
    var contentType: GTR.ContentType { get }
    var parameters: [String: Encodable]? { get }
}

extension Race {
    public var timeout: UInt32 {
        return 10
    }

    public var headers: [String: Encodable]? {
        return nil
    }

    public var method: GTR.Method {
        return .get
    }

    public var contentType: GTR.ContentType {
        return .json
    }

    public var parameters: [String: Encodable]? {
        return nil
    }
}

public protocol LongDistanceRace: Race {
    var filePath: String { get }
    var progress: ((_ now: UInt64, _ total: UInt64) -> Void)? { get }
}

extension LongDistanceRace {
    public var progress: ((UInt64, UInt64) -> Void)? { return nil }
}

public func race(race: Race, complete: @escaping (GTR.Result)) -> UInt32 {
    return GTR.request(
            method: race.method,
            url: race.url,
            contentType: race.contentType,
            headers: race.headers,
            timeOut: race.timeout,
            param: race.parameters,
            complete: complete)
}

public func longDistanceRace(race: LongDistanceRace, complete: @escaping (GTR.Result)) -> UInt32 {
    assert((race.method == GTR.Method.download || race.method == GTR.Method.upload), "method must be download or upload")
    return GTR.request(
            method: race.method,
            url: race.url,
            contentType: race.contentType,
            headers: race.headers,
            timeOut: race.timeout,
            param: race.parameters,
            downloadPath: race.filePath,
            progress: race.progress,
            complete: complete)
}
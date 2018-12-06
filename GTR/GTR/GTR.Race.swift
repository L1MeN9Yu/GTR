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

extension GTR {
    public class func race(request: Race, complete: @escaping (GTR.Result)) -> UInt32 {
        return GTR.request(
                method: request.method,
                url: request.url,
                contentType: request.contentType,
                headers: request.headers,
                timeOut: request.timeout,
                param: request.parameters,
                complete: complete)
    }
}

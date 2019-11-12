//
// Created by Mengyu Li on 2019/11/11.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public typealias Response = Goal

public struct Goal {
    public let header: Data?
    public let body: Data
    public let info: ResponseInfo?
}

public extension Goal {
    var bodyString: String? {
        String(data: body, encoding: .utf8)
    }

    var headerString: String? {
        guard let headerData = header else { return nil }
        return String(data: headerData, encoding: .utf8)
    }
}

extension Goal: CustomStringConvertible {
    public var description: String {
        """
        header : \(headerString ?? "")

        body : \(bodyString ?? "")

        info: 
        \(info?.description ?? "")
        """
    }
}

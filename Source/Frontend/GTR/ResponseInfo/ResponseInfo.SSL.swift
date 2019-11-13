//
// Created by Mengyu Li on 2019/11/13.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension ResponseInfo {
    struct SSL: Codable {
        public let verifyResult: Bool
        public let engines: [String]

        enum CodingKeys: String, CodingKey {
            case verifyResult = "SSL_VERIFY_RESULT"
            case engines = "SSL_ENGINES"
        }
    }
}

extension ResponseInfo.SSL: CustomStringConvertible {
    public var description: String {
        """
        verifyResult : \(verifyResult)
        engines : \(engines)
        """
    }
}
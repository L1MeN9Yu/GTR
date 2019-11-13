//
// Created by Mengyu Li on 2019/11/13.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension ResponseInfo {
    struct Socket: Codable {
        public let primaryIP: String
        public let primaryPort: Int
        public let localIP: String
        public let localPort: Int

        enum CodingKeys: String, CodingKey {
            case primaryIP = "PRIMARY_IP"
            case primaryPort = "PRIMARY_PORT"
            case localIP = "LOCAL_IP"
            case localPort = "LOCAL_PORT"
        }
    }
}

extension ResponseInfo.Socket: CustomStringConvertible {
    public var description: String {
        """
        primaryIP : \(primaryIP)
        primaryPort : \(primaryPort)
        localIP : \(localIP)
        localPort : \(localPort)
        """
    }
}
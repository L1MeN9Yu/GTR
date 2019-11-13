//
// Created by Mengyu Li on 2019/11/13.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension ResponseInfo {
    struct Base: Codable {
        public let responseCode: Int
        public let `protocol`: Int
        public let scheme: String
        public let effectiveURL: String
        public let connectCode: Double
        public let httpVersion: Double
        public let redirectCount: Int
        public let redirectURL: String
        public let connectCount: Int
        public let conditionUnmet: Int

        enum CodingKeys: String, CodingKey {
            case responseCode = "RESPONSE_CODE"
            case `protocol` = "PROTOCOL"
            case scheme = "SCHEME"
            case effectiveURL = "EFFECTIVE_URL"
            case connectCode = "CONNECT_CODE"
            case httpVersion = "HTTP_VERSION"
            case redirectCount = "REDIRECT_COUNT"
            case redirectURL = "REDIRECT_URL"
            case connectCount = "CONNECT_COUNT"
            case conditionUnmet = "CONDITION_UNMET"
        }
    }
}

extension ResponseInfo.Base: CustomStringConvertible {
    public var description: String {
        """
        responseCode : \(responseCode)
        protocol : \(`protocol`)
        scheme : \(scheme)
        effectiveURL : \(effectiveURL)
        connectCode : \(connectCode)
        httpVersion : \(httpVersion)
        redirectCount : \(redirectCount)
        redirectURL : \(redirectURL)
        connectCount : \(connectCount)
        conditionUnmet : \(conditionUnmet)
        """
    }
}
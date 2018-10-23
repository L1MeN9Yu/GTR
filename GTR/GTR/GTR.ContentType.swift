//
// Created by Mengyu Li on 2018-10-23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

// MARK: - Content Type
extension GTR {
    public enum ContentType: UInt {
        case json
        case formURLEncoded
        case propertyList

        func toHeader() -> [String: Encodable] {
            switch self {
            case .json:
                return ["Content-Type": "application/json; charset=utf-8"]
            case .formURLEncoded:
                return ["Content-Type": "application/x-www-form-urlencoded; charset=utf-8"]
            case .propertyList:
                return ["Content-Type": "application/x-plist; charset=utf-8"]
            }
        }
    }
}
//
// Created by Mengyu Li on 2018-10-23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//


// MARK: - Http Method
public enum Method {
    case get
    case post
    case custom(String)

    var stringValue: String {
        switch self {
        case .get:
            return "GET"
        case .post:
            return "POST"
        case .custom(let custom):
            return custom
        }
    }
}
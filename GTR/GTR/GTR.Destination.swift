//
// Created by Mengyu Li on 2018-10-23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Swift
import Foundation

// MARK: - Destination
extension GTR {
    public enum Destination {
        case win(responseData: Foundation.Data)
        case lose(httpResponseCode: Int, errorCode: Int32, errorMessage: String)
    }
}

// MARK: - Typealias
extension GTR {
    public typealias Complete = (GTR.Destination) -> Void
}

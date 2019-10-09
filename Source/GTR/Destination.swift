//
// Created by Mengyu Li on 2018-10-23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Swift
import Foundation

// MARK: - Destination
public enum Destination {
    case win(httpHeader: HttpHeader?, responseData: Foundation.Data)
    case lose(RaceError)
}

// MARK: - Typealias
public typealias Result = (Destination) -> Void

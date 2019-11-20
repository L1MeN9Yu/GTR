//
// Created by Mengyu Li on 2019/11/20.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public enum Mime {
    case text(_ name: String, _ value: String)
    case file(_ name: String, _ file: URL)
}

public extension Mime {
    var type: CInt {
        switch self {
        case .text(_, _):
            return 0
        case .file(_, _):
            return 1
        }
    }
}
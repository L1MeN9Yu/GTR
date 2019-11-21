//
// Created by Mengyu Li on 2019/11/21.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension Option {
    enum TimeCondition {
        case ifModifySince(_ time: CLongLong)
        case ifUnmodifiedSince(_ time: CLongLong)
        case lastModify(_ time: CLongLong)

        var intValue: Int32 {
            switch self {
            case .ifModifySince(_): return 1
            case .ifUnmodifiedSince(_): return 2
            case .lastModify(_): return 3
            }
        }

        var timeValue: Int64 {
            switch self {
            case .ifModifySince(let time): return time
            case .ifUnmodifiedSince(let time): return time
            case .lastModify(let time): return time
            }
        }
    }
}

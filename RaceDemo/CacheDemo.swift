//
// Created by Mengyu Li on 2019/11/21.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct CacheDemo: DataTask {
    private(set) var url: String = "https://httpbin.org/cache"
    private(set) var timeCondition: Option.TimeCondition? = .ifModifySince(CLongLong(Date().timeIntervalSince1970))
}

//
// Created by Mengyu Li on 2019-08-08.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct GetCacheDemo: Race {
    let url: String = "https://httpbin.org/cache"
}

extension GetCacheDemo {
    @discardableResult
    static func fetch(complete: @escaping GTR.Result) -> UInt32 {
        let demo = GetCacheDemo()
        return race(race: demo, complete: complete)
    }
}
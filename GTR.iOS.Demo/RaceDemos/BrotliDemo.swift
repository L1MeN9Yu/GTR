//
// Created by Mengyu Li on 2019-08-06.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import protocol GTR.Race
import typealias GTR.Result;
import func GTR.race;

struct BrotliDemo: Race {
    let url: String = "https://httpbin.org/brotli"
    let timeout: UInt32 = 10
}

extension BrotliDemo {
    @discardableResult
    static func fetch(complete: @escaping GTR.Result) -> UInt32 {
        let brotliDemo = BrotliDemo()
        return race(race: brotliDemo, complete: complete)
    }
}
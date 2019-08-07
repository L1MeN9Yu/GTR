//
// Created by Mengyu Li on 2018-12-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct GetDemo: Race {

    var url: String {
        return "https://httpbin.org/get"
    }

    var timeout: UInt32 {
        return 10
    }

    var method: GTR.Method {
        return .get
    }
}

extension GetDemo {
    @discardableResult
    static func fetch(complete: @escaping GTR.Result) -> UInt32 {
        let race = GetDemo()
        return GTR.race(race: race, complete: complete)
    }
}

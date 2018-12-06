//
// Created by Mengyu Li on 2018-12-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct RaceDemo: Race {
    let language: String
    let pageNo: Int = 1
    let pageSize: Int = 20

    var url: String {
        return "http://dict.hjapi.com/v10/read/news/listByPage"
    }

    var timeout: UInt32 {
        return 10
    }

    var method: GTR.Method {
        return .post
    }

    var parameters: [String: Encodable]? {
        return ["language": self.language, "pageNo": self.pageNo, "pageSize": self.pageSize]
    }
}

extension RaceDemo {
    @discardableResult
    static func fetch(language: String, complete: @escaping GTR.Result) -> UInt32 {
        let race = RaceDemo(language: language)
        return GTR.race(request: race, complete: complete)
    }
}

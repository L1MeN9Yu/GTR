//
// Created by Mengyu Li on 2018-12-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct GetDemo: Race {
    var url: String { "https://httpbin.org/get?555=999" }
    private(set) var parameters: [String: Any]? = [
        "1": "2",
        "3": 456,
    ]
}

//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct CustomDemo: Race {
    let url: String = "https://httpbin.org/patch"
    let method: GTR.Method = .custom("PATCH")
    let parameters: [String: Any]? = [
        "name": "FUCK"
    ]
}

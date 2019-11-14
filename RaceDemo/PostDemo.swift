//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct PostDemo: Race {
    let url: String = "https://httpbin.org/post"
    let method: GTR.Method = .post
    let parameters: [String: Any]? = ["a": 1, "b": "boy"]

    let options = TaskOptions(isDebug: true, timeout: 10)
}

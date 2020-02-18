//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct PostDemo: Race {
//    let url: String = "https://httpbin.org/post"
    let url: String = "http://101.37.133.116/trace_device"
    let method: GTR.Method = .post
    let parameters: [String: Any]? = ["beat_gap": 15]

//    let options = TaskOptions(isDebug: true, timeout: 10)
}

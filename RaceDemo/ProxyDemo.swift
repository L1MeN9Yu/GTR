//
// Created by Mengyu Li on 2019/11/12.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct ProxyDemo: Race {
    let url: String = "https://httpbin.org/get"
    let proxy: (String, Int)? = ("http://127.0.0.1", 1087)
}

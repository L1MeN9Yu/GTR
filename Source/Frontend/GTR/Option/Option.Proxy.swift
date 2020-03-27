//
// Created by Mengyu Li on 2019/11/14.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

public extension Option {
    struct Proxy {
        public let url: String
        public let port: Int

        public init(url: String, port: Int) {
            self.url = url
            self.port = port
        }
    }
}
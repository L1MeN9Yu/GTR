//
// Created by Mengyu Li on 2018-10-23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//


// MARK: - Http Method
extension GTR {
    public enum Method: UInt {
        case get
        case post
        /// 像Post一样请求,对服务端而言PUT是幂等的,如需上传文件,用upload
        case put
        case download
        case upload

        static let userInfoKey = "GTR.Method"
    }
}
//
// Created by Mengyu Li on 2019-08-08.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

private(set) var __jsonEncoder: JSONEncoder = {
    let encoder = JSONEncoder()
    return encoder
}()

private(set) var __jsonDecoder: JSONDecoder = {
    let decoder = JSONDecoder()
    return decoder
}()

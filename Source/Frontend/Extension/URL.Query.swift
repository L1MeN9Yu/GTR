//
// Created by Mengyu Li on 2019/10/25.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

extension URL {
    func appendingQueryParameters(_ parameters: [String: String]) -> URL {
        var urlComponents = URLComponents(url: self, resolvingAgainstBaseURL: true)
        var items = urlComponents?.queryItems ?? []
        items += parameters.map({ URLQueryItem(name: $0, value: $1) })
        urlComponents?.queryItems = items
        return urlComponents?.url ?? self
    }

    mutating func appendQueryParameters(_ parameters: [String: String]) {
        self = appendingQueryParameters(parameters)
    }
}
//
// Created by Mengyu Li on 2019/11/12.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

struct Coder {
    private init() {}
}

extension Coder {
    private static var encoder: JSONEncoder = {
        let encoder = JSONEncoder()
        return encoder
    }()

    private static var decoder: JSONDecoder = {
        let decoder = JSONDecoder()
        return decoder
    }()
}

extension Coder {
    static func decode<T: Decodable>(data: Data) -> T? {
        #if DEBUG
        do {
            return try decoder.decode(T.self, from: data)
        } catch let error {
            print("\(error)")
            return nil
        }
        #else
        return try? decoder.decode(T.self, from: data)
        #endif
    }

    static func encode<T: Encodable>(value: T) -> Data? {
        #if DEBUG
        do {
            return try encoder.encode(value)
        } catch let error {
            print("\(error)")
            return nil
        }
        #else
        return try? encoder.encode(value)
        #endif
    }
}

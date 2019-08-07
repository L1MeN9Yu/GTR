//
// Created L1MeN9Yu on 2018/5/20.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

extension Dictionary {
    func jsonStringEncoded() -> String? {
        if JSONSerialization.isValidJSONObject(self) {
            do {
                let jsonData = try JSONSerialization.data(withJSONObject: self)
                let jsonString = String(data: jsonData, encoding: .utf8)
                return jsonString
            } catch {
                return nil
            }
        }

        return nil
    }

    func jsonPrettyStringEncoded() -> String? {
        if JSONSerialization.isValidJSONObject(self) {
            do {
                let jsonData = try JSONSerialization.data(withJSONObject: self, options: .prettyPrinted)
                let jsonString = String(data: jsonData, encoding: .utf8)
                return jsonString
            } catch {
                return nil
            }
        }

        return nil
    }

    func toData() -> Data? {
        if JSONSerialization.isValidJSONObject(self) {
            do {
                let jsonData = try JSONSerialization.data(withJSONObject: self)
                return jsonData
            } catch {
                return nil
            }
        }
        return nil
    }
}

extension Dictionary {
    var queryString: String {
        var output: String = ""
        for (key, value) in self {
            output += "\(key)=\(value)&"
        }
        output = String(output.dropLast())
        return output
    }
}
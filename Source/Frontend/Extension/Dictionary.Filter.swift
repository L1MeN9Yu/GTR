//
// Created by Mengyu Li on 2019/10/25.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

extension Dictionary {
    func filterValue<T>(valueType: T.Type) -> [Key: T]? {
        let result = self.filter { (key: Key, value: Value) in
            value is T
        } as? [Key: T]
        return result
    }
}
//
// Created by Mengyu Li on 2019/11/20.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct FormDataDemo: Race {
    let url: String = "url"
    let method: GTR.Method = .post
    let contentType: ContentType = .formData
    let parameters: [String: Any]? = [
        ContentType.FormDataKey : [
            Mime.file("file", URL(fileURLWithPath: "file path")),
        ]
    ]
}

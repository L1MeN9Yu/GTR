//
// Created by Mengyu Li on 2018-12-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

public enum GTRError: Error {
    case raceFailed(httpResponseCode: Int, errorCode: Int32, errorMessage: String)
}

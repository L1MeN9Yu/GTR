//
// Created by Mengyu Li on 2018-10-23.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Swift
import Foundation

public typealias Destination = Swift.Result<Goal, RaceError>

// MARK: - Typealias
public typealias Result = (Destination) -> Void

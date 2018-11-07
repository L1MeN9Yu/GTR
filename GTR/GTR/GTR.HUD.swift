//
// Created by Mengyu Li on 2018-11-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

extension GTR {
    class HUD {
        private let lock = NSLock()

        private var startTimer: Timer?
        private var completeTimer: Timer?

        private var activityCount: Int = 0
        private var enabled: Bool = true

        var startDelay: TimeInterval = 1.0

        var completionDelay: TimeInterval = 0.2

        private var activityIndicatorState: ActivityIndicatorState = .notActive {
            didSet {
                switch activityIndicatorState {
                case .notActive:
                    isNetworkActivityIndicatorVisible = false
                    invalidateTimers()
                case .delayingStart:
                    scheduleStartDelayTimer()
                case .active:
                    invalidateTimers(timerType: TimerType.Complete)
                    isNetworkActivityIndicatorVisible = true
                case .delayingCompletion:
                    scheduleCompletionDelayTimer()
                }
            }
        }

        /// A boolean value indicating whether the network activity indicator is currently visible.
        public private(set) var isNetworkActivityIndicatorVisible: Bool = false {
            didSet {
                guard isNetworkActivityIndicatorVisible != oldValue else { return }

                DispatchQueue.main.async {
                    UIApplication.shared.isNetworkActivityIndicatorVisible = self.isNetworkActivityIndicatorVisible
                    self.networkActivityIndicatorVisibilityChanged?(self.isNetworkActivityIndicatorVisible)
                }
            }
        }

        var networkActivityIndicatorVisibilityChanged: ((Bool) -> Void)?

        init() {
            registerNotifications()
        }

        deinit {
            unregisterNotifications()
            invalidateTimers()
        }
    }
}

// MARK: - Private
extension GTR.HUD {
    private func registerNotifications() {
        let notificationCenter = NotificationCenter.default

        notificationCenter.addObserver(
                self,
                selector: #selector(GTR.HUD.networkRequestDidStart),
                name: GTR.Notification.RequestStatusType.start.notificationName,
                object: nil
        )

        notificationCenter.addObserver(
                self,
                selector: #selector(GTR.HUD.networkRequestDidComplete),
                name: GTR.Notification.RequestStatusType.canceled.notificationName,
                object: nil
        )

        notificationCenter.addObserver(
                self,
                selector: #selector(GTR.HUD.networkRequestDidComplete),
                name: GTR.Notification.RequestStatusType.complete.notificationName,
                object: nil
        )
    }

    private func unregisterNotifications() {
        NotificationCenter.default.removeObserver(self)
    }

    private func invalidateTimers(timerType: TimerType = [TimerType.Start, TimerType.Complete]) {
        if timerType.contains(.Start) {
            startTimer?.invalidate()
            startTimer = nil
        }

        if timerType.contains(.Complete) {
            completeTimer?.invalidate()
            completeTimer = nil
        }
    }

    private func updateActivityIndicatorStateForNetworkActivityChange() {
        guard enabled else { return }

        switch activityIndicatorState {
        case .notActive:
            if activityCount > 0 { activityIndicatorState = .delayingStart }
        case .delayingStart:
            do {}
        case .active:
            if activityCount == 0 { activityIndicatorState = .delayingCompletion }
        case .delayingCompletion:
            if activityCount > 0 { activityIndicatorState = .active }
        }
    }

    private func incrementActivityCount() {
        lock.lock();
        defer { lock.unlock() }

        activityCount += 1
        updateActivityIndicatorStateForNetworkActivityChange()
    }

    private func decrementActivityCount() {
        lock.lock();
        defer { lock.unlock() }

        activityCount -= 1
        updateActivityIndicatorStateForNetworkActivityChange()
    }

    private func scheduleStartDelayTimer() {
        let timer = Timer(
                timeInterval: startDelay,
                target: self,
                selector: #selector(GTR.HUD.startTimerFired),
                userInfo: nil,
                repeats: false
        )

        DispatchQueue.main.async {
            RunLoop.main.add(timer, forMode: .common)
            RunLoop.main.add(timer, forMode: .tracking)
        }

        startTimer = timer
    }

    private func scheduleCompletionDelayTimer() {
        let timer = Timer(
                timeInterval: completionDelay,
                target: self,
                selector: #selector(GTR.HUD.completeTimerFired),
                userInfo: nil,
                repeats: false
        )

        DispatchQueue.main.async {
            RunLoop.main.add(timer, forMode: .common)
            RunLoop.main.add(timer, forMode: .tracking)
        }

        completeTimer = timer
    }

    @objc
    private func startTimerFired() {
        lock.lock()
        defer { lock.unlock() }

        activityIndicatorState = activityCount > 0 ? .active : .notActive
    }

    @objc
    private func completeTimerFired() {
        lock.lock();
        defer { lock.unlock() }

        activityIndicatorState = .notActive
    }

    @objc
    private func networkRequestDidStart() {
        incrementActivityCount()
    }

    @objc
    private func networkRequestDidComplete() {
        decrementActivityCount()
    }
}

// MARK: - ActivityIndicatorState
extension GTR.HUD {
    private enum ActivityIndicatorState {
        case notActive, delayingStart, active, delayingCompletion
    }
}

extension GTR.HUD {
    private struct TimerType: OptionSet {
        let rawValue: UInt

        static let Start = TimerType(rawValue: 1 << 0)
        static let Complete = TimerType(rawValue: 1 << 1)
    }
}
//
//  ViewController.swift
//  GTRDemo
//
//  Created L1MeN9Yu on 2018/8/30.
//  Copyright © 2018年 limengyu.top. All rights reserved.
//

import UIKit
import GTR

class ViewController: UIViewController {

    private lazy var actionButton = { () -> UIButton in
        let actionButton = UIButton(type: .custom)
        actionButton.setTitle("Go", for: .normal)
        actionButton.setTitleColor(.black, for: .normal)
        actionButton.addTarget(self, action: #selector(ViewController.getAction), for: .touchUpInside)
        actionButton.translatesAutoresizingMaskIntoConstraints = false
        return actionButton
    }()

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.

        self.setupUI()
        self.setupGTR()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

// MARK: - Actions
extension ViewController {
    @objc
    private func getAction(button: UIButton) {
        self.get()
        self.put()

//        self.download()
    }
}

// MARK: - Private
extension ViewController {
    private func setupUI() {
        self.view.addSubview(self.actionButton)
        self.actionButton.leadingAnchor.constraint(equalTo: self.view.leadingAnchor).isActive = true
        self.actionButton.bottomAnchor.constraint(equalTo: self.bottomLayoutGuide.topAnchor).isActive = true
        self.actionButton.trailingAnchor.constraint(equalTo: self.view.trailingAnchor).isActive = true
        self.actionButton.heightAnchor.constraint(equalToConstant: 60).isActive = true
    }

    private func setupGTR() {
        GTR.setup(driver: type(of: self), horn: type(of: self))
    }

    private func get() {
        RaceDemo.fetch() { destination in
            switch destination {
            case .win(let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string)")
                }
                break
            case .lose(let httpResponseCode, let errorCode, let errorMessage):
                break
            }
        }
    }

    private func put() {
        CompleteLearning.put(courseID: 37, lessonId: 8832301, learningSeconds: 106) { destination in
            switch destination {
            case .win(let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string)")
                }
                break
            case .lose(let httpResponseCode, let errorCode, let errorMessage):
                break
            }
        }
    }

    private func download() {
        DownloadDemo.request(progress: { now, total in
            print("now = \(now) || total = \(total)")
        }, complete: { destination in
            switch destination {
            case .win(let responseData):
                return
            case .lose(let httpResponseCode, let errorCode, let errorMessage):
                return
            }
        })
    }
}

extension ViewController: Driver {
    public static var identity: () -> [String: Encodable]? {
        return {
            return ["Access-Token": "000347174c.db8743c66094ff2964a2b4b6791cf5db"]
        }
    }
    public static var userAgent: () -> String? {
        return {
            return "GTRDemo/1.0.0"
        }
    }
}

extension ViewController: Horn {
    public class func whistle(type: HornType, message: String, filename: String, function: String, line: Int) {
        print("[\(type.name)] => \(message)")
    }
}



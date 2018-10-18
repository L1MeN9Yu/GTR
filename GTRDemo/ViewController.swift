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
    @objc private func getAction(button: UIButton) {
        GTR.race(method: .post, url: "http://dict.hjapi.com/v10/read/news/listByPage", timeOut: 10,
                param: ["language": "en", "pageNo": 1, "pageSize": 20]) { destination in
            switch destination {
            case .win(let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string.count)")
                }
                break
            case .lose(let httpResponseCode, let errorCode, let errorMessage):
                break
            }
        }
    }
}

// MARK: - Private
extension ViewController {
    private func setupUI() {
        let screenHeight = UIScreen.main.bounds.size.height
        let screenWidth = UIScreen.main.bounds.size.width
        let buttonHeight = CGFloat(60)
        self.actionButton.frame = CGRect(x: 0, y: screenHeight - 60, width: screenWidth, height: buttonHeight)
        self.view.addSubview(self.actionButton)
    }

    private func setupGTR() {
        GTR.setup(driver: self, hornType: ViewController.self)
    }
}

extension ViewController: Driver {
    public var identity: () -> [String: Encodable]? {
        return {
            return nil
        }
    }
    public var userAgent: () -> String? {
        return {
            return "GTRDemo/1.0.0"
        }
    }
}

extension ViewController: Horn {
//    public class func whistle(message: String, filename: String, function: String, line: Int) {
//        print("gtr message : " + "\n" + "\(message)" + "\n");
//    }
}



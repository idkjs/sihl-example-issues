let name = "Issue Management App";

let namespace = "issues";

let config = () =>
  Sihl.Core.Config.Schema.[
    string_(
      ~default="console",
      ~choices=["smtp", "console", "memory"],
      "EMAIL_BACKEND",
    ),
    string_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_HOST"),
    int_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_PORT"),
    string_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_AUTH_USERNAME"),
    string_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_AUTH_PASSWORD"),
    bool_(~default=false, "SMTP_SECURE"),
    bool_(~default=false, "SMTP_POOL"),
  ];

let endpoints = () =>
  Handler.[
    GetBoardsByUser.handler,
    GetIssuesByBoard.handler,
    AddBoard.handler,
    AddIssue.handler,
    CompleteIssue.handler,
    AdminUi.Issues.handler,
    AdminUi.Boards.handler,
    Client.app_handler,
    Client.root_handler,
  ];

let repos = () => {
  [((module Repository): (module Sihl.Core.Contract.REPOSITORY))];
};

let bindings = () => [];

let commands = () => [];

let start = () => Ok();

let stop = () => Ok();

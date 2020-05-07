// TODO remove this and use project definition from Project.re

let config =
  Sihl_core.Config.Setting.create(
    ~development=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_SENDER", "josef@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
      ("EMAIL_BACKEND", "console"),
    ],
    ~test=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_SENDER", "josef@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
      ("EMAIL_BACKEND", "memory"),
    ],
    ~production=[
      ("EMAIL_BACKEND", "smtp"),
      ("BASE_URL", "https://sihl-example-issues.oxidizing.io"),
      ("SMTP_SECURE", "false"),
      ("SMTP_HOST", "smtp.sendgrid.net"),
      ("SMTP_PORT", "587"),
      ("SMTP_AUTH_USERNAME", "apikey"),
    ],
  );

let project =
  Sihl_core.Run.Project.create(
    ~config,
    [(module Sihl_users.App), (module Sihl_example_issues.App)],
  );

let () = Sihl_core.Run.Project.run_command(project);

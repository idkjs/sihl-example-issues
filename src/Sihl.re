module Core = SihlCore.Api.Core;
module App = SihlCore.Api.MakeApp(SihlMysql.Mysql.Persistence);
module Users = SihlUsers.SihlUsers;

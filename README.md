# GfiRatServer

QQ 84271643

IDR_LIST MENU
BEGIN
    POPUP "MENU"
    BEGIN
        POPUP "小功能(&1)"
        BEGIN
            MENUITEM "桌面预览(&1)",                    ID_DESK_LOOK
        END
        MENUITEM "文件管理(&F)",                    IDM_FILEMANAGER
        MENUITEM "键盘记录(&K)",                    IDM_KEYBOARD
        POPUP "屏幕控制(&C)"
        BEGIN
            MENUITEM "老屏幕",                         IDM_SCREENSPY
            MENUITEM "新屏幕",                         IDM_newSCREENSPY
        END
        MENUITEM "系统管理(&M)",                    IDM_SYSTEM
        MENUITEM "视频查看(&V)",                    IDM_WEBCAM
        MENUITEM "语音监听(&W)",                    IDM_AUDIO_LISTEN
        MENUITEM "音响监听(&I)",                    IDM_SPEAKER_LISTEN
        MENUITEM "远程终端(&T)",                    IDM_REMOTESHELL
        MENUITEM "主机管理(&H)",                    IDM_SYSINFO
        MENUITEM "注  册  表(&R)",                 IDM_REGEDIT
        MENUITEM "代理映射(&p)",                    IDM_PROXY
        MENUITEM "服务管理(&G)",                    IDM_SERMANAGER
        POPUP "筛选功能(&D)"
        BEGIN
            MENUITEM "进程(&P)",                      ID_JINCHENG
            MENUITEM "窗口(&W)",                      ID_CHUANGKOU
            MENUITEM "重置(&R)",                      ID_CHONGZHI
        END
        POPUP "其它功能(&O)"
        BEGIN
            MENUITEM "下载执行(&D)",                    IDM_DOWNEXEC
            POPUP "访问网址(&V)"
            BEGIN
                MENUITEM "隐藏(&H)",                      IDM_OPEN_URL_HIDE
                MENUITEM "显示(&S)",                      IDM_OPEN_URL_SHOW
            END
            POPUP "清理日志(&C)"
            BEGIN
                MENUITEM "清除全部日志(&A)",                  IDM_CLEANEVENT
                MENUITEM "理系统日志(&S)",                   IDM_CLEAN_System
                MENUITEM "清除安全日志(&Q)",                  IDM_CLEAN_Security
                MENUITEM "清除应用程序(&Y)",                  IDM_CLEAN_Application
            END
            MENUITEM SEPARATOR
            MENUITEM "下载更新(&U)",                    IDM_UPDATE_SERVER
            MENUITEM SEPARATOR
            MENUITEM "消息发送",                        IDM_SHOWMSG
            MENUITEM "远程交谈",                        IDM_CHAT
        END
        MENUITEM SEPARATOR
        POPUP "复制信息(&p)"
        BEGIN
            MENUITEM "复制IP(&I)",                    IDM_IP
            MENUITEM "地理位置(&D)",                    IDM_DILIXINXI
            MENUITEM "所有信息(&A)",                    IDM_ALL
        END
        POPUP "会话管理(&S)"
        BEGIN
            MENUITEM "卸载(&U)",                      IDM_REMOVE
            MENUITEM SEPARATOR
            MENUITEM "注销(&L)",                      IDM_LOGOFF
            MENUITEM "重启(&R)",                      IDM_REBOOT
            MENUITEM "关机(&S)",                      IDM_SHUTDOWN
        END
        MENUITEM "更改备注(&E)",                    IDM_RENAME_REMARK
        MENUITEM "更改分组(&B)",                    IDM_CHANGE_GROUP
        MENUITEM SEPARATOR
        MENUITEM "选择全部(&A)",                    IDM_SELECT_ALL
        MENUITEM "取消选择(&U)",                    IDM_UNSELECT_ALL
    END
END

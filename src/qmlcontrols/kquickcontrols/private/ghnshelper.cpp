#include "ghnshelper.h"

#include <QQuickItem>
#include <QQuickWindow>

#include <KNewStuff3/KNS3/DownloadDialog>

GHNSHelper::GHNSHelper(QObject *parent)
    : QObject(parent)
{

}

GHNSHelper::~GHNSHelper()
{
    if (m_newStuffDialog) {
        m_newStuffDialog->close();
    }
}

void GHNSHelper::getNewStuff(QQuickItem *ctx)
{
    if (m_ksnrc.isEmpty()) {
        return;
    }
    if (!m_newStuffDialog) {
        m_newStuffDialog = new KNS3::DownloadDialog(m_ksnrc);
        if (!m_title.isEmpty()) {
            m_newStuffDialog->setTitle(m_title);
        }
        connect(m_newStuffDialog.data(), &QDialog::accepted, this, [this]() {
           if (m_newStuffDialog->changedEntries().size() > 0) {
                emit entriesChanged();
           }
        });
    }

    if (ctx && ctx->window()) {
        m_newStuffDialog->setWindowModality(Qt::WindowModal);
        m_newStuffDialog->winId(); // so it creates the windowHandle();
        m_newStuffDialog->windowHandle()->setTransientParent(ctx->window());
    }

    m_newStuffDialog->show();
}

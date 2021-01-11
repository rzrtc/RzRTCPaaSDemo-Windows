#include "ListWidgetSet.h"
#include <QPushButton>
ListWidgetSet::ListWidgetSet(QObject *parent)
	: QObject(parent)
{
}

ListWidgetSet::~ListWidgetSet()
{
}

void ListWidgetSet::LeftSetList(QListWidget* lst, QString iconFile, QString iconText, QString valueText, QString buttonImage, QString valueTextSheetStyle)
{
	//ui.listWidget_setup_channel->setItemDelegate(new NoFocusDelegate());
	//ui.listWidget_setup_channel->horizontalHeader()->setStretchLastSection(true);
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//ui.listWidget_setup_channel->setFrameShape(QListWidget::NoFrame);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background:transparent;");

	QLabel* typeLabel = new QLabel(widget);
	QPixmap myPix("./resource/image/" + iconFile);
	typeLabel->setObjectName("typeLabel");
	typeLabel->setFixedSize(24, 24);
	typeLabel->setPixmap(myPix);
	typeLabel->setAlignment(Qt::AlignCenter);
	//TypeLabel->setScaledContents(true);
	typeLabel->setStyleSheet("QLabel{border-radius:4px;  background-color:#1e6ceb;}");

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(169, 22);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text - align: left; color:#64728c;}");

	QLabel* valueTextLabel = new QLabel(widget);
	valueTextLabel->setObjectName("valueTextLabel");
	valueTextLabel->setFixedSize(118, 22);
	valueTextLabel->setText(valueText);
	valueTextLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	//textLabel2->setScaledContents(true);
	valueTextLabel->setStyleSheet(valueTextSheetStyle);

	QLabel* backLabel = new QLabel(widget);
	QPixmap backPix("./resource/image/" + buttonImage);
	backLabel->setObjectName("backLabel");
	backLabel->setFixedSize(24, 24);
	backLabel->setPixmap(backPix);
	backLabel->setScaledContents(true);
	backLabel->setStyleSheet("QLabel{border-radius:4px; background-color:#1e6ceb;}");
	backLabel->setStyleSheet("QPushButton{border: none; background-color:white; }");
	backLabel->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

	QHBoxLayout* horLayout = new QHBoxLayout;
	horLayout->setContentsMargins(0, 0, 0, 0);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->addWidget(typeLabel);
	horLayout->addWidget(titleTextLabel);
	horLayout->addWidget(valueTextLabel);
	horLayout->addWidget(backLabel);
	widget->setLayout(horLayout);

	/*int left, top,right, bottom;
	horLayout->getContentsMargins(&left, &top, &right, &bottom);
	horLayout->setContentsMargins(4, top, 4, bottom);*/
	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem();

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(375, 44));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 44);
	lst->setItemWidget(item, widget);

}

void ListWidgetSet::SetList(QListWidget* lst, QString iconText, QString iconfile, int labelWidth)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background:transparent;");

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(labelWidth, 22);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c;}");

	QLabel* backLabel = new QLabel(widget);
	QPixmap backPix("./resource/image/" + iconfile);
	backLabel->setObjectName("backLabel");
	backLabel->setFixedSize(24, 24);
	backLabel->setPixmap(backPix);
	backLabel->setScaledContents(true);
	backLabel->setStyleSheet("background:transparent;");
	backLabel->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

	QHBoxLayout* horLayout = new QHBoxLayout;
	
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	
	horLayout->setContentsMargins(16, 0, 16, 0);
	horLayout->addWidget(titleTextLabel);
	//horLayout->addSpacing(spacing);
	horLayout->addWidget(backLabel);
	horLayout->addSpacing(20);
	widget->setLayout(horLayout);

	//backLabel->clear();
	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem();

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(375, 44));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 44);
	lst->setItemWidget(item, widget);
}

void ListWidgetSet::SetListTitle(QListWidget* lst, QString iconText)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background-color: #f7f8fa;");

	QLabel* textLabel = new QLabel(widget);
	textLabel->setObjectName("textLabel");
	textLabel->setFixedSize(169, 22);
	textLabel->setText(iconText);
	textLabel->setScaledContents(true);
	textLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#cfd3de;}");

	QHBoxLayout* horLayout = new QHBoxLayout;
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 0, 0);
	horLayout->addWidget(textLabel);
	horLayout->addSpacing(184);
	widget->setLayout(horLayout);

	//backLabel->clear();
	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem();
	item->setFlags(Qt::NoItemFlags);
	QSize size = item->sizeHint();
	item->setSizeHint(QSize(375, 44));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 44);
	lst->setItemWidget(item, widget);
}

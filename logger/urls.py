# accounts/urls.py
from django.urls import path
from . import views
from django.conf.urls import url

urlpatterns = [
    path('', views.add_logger.as_view()),
]

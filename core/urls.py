# accounts/urls.py
from django.urls import path
from . import views
from django.conf.urls import url

urlpatterns = [
    path('add_ip/', views.add_IP.as_view()),
]

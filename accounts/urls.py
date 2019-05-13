# accounts/urls.py
from django.urls import path
from . import views
from django.contrib.auth import views as auth_views
from django.conf.urls import url

urlpatterns = [
    path('accounts/login/', auth_views.LoginView.as_view()),
    path('signup/', views.SignUp.as_view(), name='signup'),
]
